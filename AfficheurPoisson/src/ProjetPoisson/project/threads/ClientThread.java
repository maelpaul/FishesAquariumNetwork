package ProjetPoisson.project.threads;
import ProjetPoisson.mightylib.resources.Resources;
import ProjetPoisson.project.client.ClientTcp;
import ProjetPoisson.project.client.Configuration;
import ProjetPoisson.project.client.Message;

public class ClientThread extends CommunicationThread {

    public static final int SLEEP_TIME = 10;
    public static final int SPECIAL_ID = -1;

    public static final String HEADER_SEPARATION = "|";

    protected ClientTcp client;
    protected boolean shouldTryConnection;

    public ClientThread() {
        this.running = true;
    }

    public void shouldTryConnection(){
        shouldTryConnection = true;
    }

    @Override
    public void setup() {
        super.setup();
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
        if (client.isConnected())
            client.closeConnection();
    }

    public void run() {
        this.setup();
        reset();

        try {
            while (this.running){
                Thread.sleep(SLEEP_TIME);
                //System.out.println(messageProcessed + " " + messageReceived + " " + messageToSend.size());

                if (client.isConnected()){
                    if (messageToSend.size() > 0 && receivedMessageNumber == processedMessageNumber) {
                        String toSend = processedMessageNumber + HEADER_SEPARATION + messageToSend.get(processedMessageNumber);

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
                            int numberMessage = Integer.parseInt(result.substring(0, result.indexOf(HEADER_SEPARATION)));
                            if (numberMessage >= receivedMessageNumber)
                                receivedMessageNumber = numberMessage + 1;

                            System.out.println("Messaged received(" + receivedMessageNumber + ") :" + result);

                            result = result.substring(result.indexOf(HEADER_SEPARATION) + 1);

                            if (numberMessage == SPECIAL_ID){
                                receivedMessages.add(new Message(SPECIAL_ID, result));
                            } else {
                                receivedMessages.add(new Message(receivedMessageNumber - 1, result));
                                messageToSend.remove(receivedMessageNumber - 1);
                            }
                        } else {
                            System.err.println("Message without header : \n" + result);
                        }
                    }
                } else if (shouldTryConnection) {
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
