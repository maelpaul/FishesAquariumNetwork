package ProjetPoisson.project.threads;
import ProjetPoisson.mightylib.resources.Resources;
import ProjetPoisson.project.client.ClientTcp;
import ProjetPoisson.project.client.Configuration;
import ProjetPoisson.project.client.Message;

public class ClientThread extends CommunicationThread {

    protected ClientTcp client;
    protected boolean shouldTryConnection;

    public ClientThread() {
        this.running = true;

        reset();
    }

    public void shouldTryConnection(){
        shouldTryConnection = true;
    }

    public void ClientSetup() {
        System.out.println("[Debug] Setting up the client...");
        Configuration conf_client = Resources.getInstance().getResource(Configuration.class, "client");
        client = new ClientTcp(conf_client);
    }


    public ClientTcp getClient() {
        return this.client;
    }

    public void reset(){
        super.reset();

        receivedMessageNumber = -1;
        processedMessageNumber = 0;

        shouldTryConnection = false;
    }

    public void run() {
        this.ClientSetup();
        reset();

        try {
            while (this.running){
                Thread.sleep(50); // Add a delay of 500 milliseconds
                //System.out.println(messageProcessed + " " + messageReceived + " " + messageToSend.size());

                if (client.isConnected()){
                    if (messageToSend.size() > 0 && receivedMessageNumber == processedMessageNumber) {
                        String toSend = processedMessageNumber + "|" + messageToSend.get(processedMessageNumber);

                        System.out.println("Messaged sent(" + receivedMessageNumber + ") :" + toSend);
                        client.sendMessage(toSend);

                        processedMessageNumber += 1;
                    }

                    String result = client.readMessage();
                    if (result != null) {
                        if (result.contains("Connection lost")){
                            this.running = false;
                        }
                        if (result.contains("|")) {
                            int numberMessage = Integer.parseInt(result.substring(0, result.indexOf("|")));
                            if (numberMessage >= receivedMessageNumber)
                                receivedMessageNumber = numberMessage + 1;

                            System.out.println("Messaged received(" + receivedMessageNumber + ") :" + result);

                            if (result.replace("\n", "").equalsIgnoreCase("-1|Timeout")
                            || result.replace("\n", "").equalsIgnoreCase("-1|Serveur fermé")){
                                System.out.println("Disconnected from the server");
                                client.closeConnection();
                                reset();
                            }

                            result = result.substring(result.indexOf("|") + 1);

                            receivedMessages.add(new Message(receivedMessageNumber - 1, result));
                            messageToSend.remove(receivedMessageNumber - 1);
                        } else {
                            System.err.println("Message without header : \n" + result);
                        }
                    }
                } else if (shouldTryConnection){
                    System.out.println("GNONENOF");
                    client.tryCreateConnection();
                    shouldTryConnection = false;
                }
            }
        } catch (InterruptedException e) {
            e.printStackTrace();
        }
    }

    @Override
    public void doStop() {
        this.running = false;
        if (client != null && client.isConnected()) {
            client.closeConnection();
        }
    }
}
