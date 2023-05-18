package ProjetPoisson.project.threads;
import java.util.ArrayList;
import java.util.concurrent.CountDownLatch;
import java.util.concurrent.TimeUnit;
import ProjetPoisson.mightylib.resources.Resources;
import ProjetPoisson.project.client.ClientTcp;
import ProjetPoisson.project.client.Configuration;
import ProjetPoisson.project.client.Message;

import java.io.*;
import java.net.Socket;
import java.net.SocketTimeoutException;
public class ClientThread extends CommunicationThread {

    protected ClientTcp client;
    protected boolean shouldTryConnection;

    public ClientThread() {
        this.running = true;
        shouldTryConnection = false;
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

    public void run() {
        this.ClientSetup();
        messageReceived -= 1;

        try {
            while (this.running){
                Thread.sleep(50); // Add a delay of 500 milliseconds
                //System.out.println(messageProcessed + " " + messageReceived );

                if (client.isConnected()){
                    if (messageToSend.size() > 0 && messageReceived == messageProcessed) {
                        String toSend = messageProcessed + "|" + messageToSend.get(messageProcessed);

                        System.out.println("Messaged sent(" + messageReceived + ") :" + toSend);
                        client.sendMessage(toSend);

                        messageProcessed += 1;
                    }

                    String result = client.readMessage();
                    if (result != null) {
                        if (result.contains("|")) {
                            int numberMessage = Integer.parseInt(result.substring(0, result.indexOf("|")));
                            if (numberMessage > messageReceived)
                                messageReceived = numberMessage;

                            System.out.println("Messaged received(" + messageReceived + ") :" + result);

                            result = result.substring(result.indexOf("|") + 1);

                            receivedMessages.add(new Message(messageReceived, result));
                            messageToSend.remove(messageReceived);

                            if (result.contains("list")) {
                                if (messageReceived != messageProcessed)
                                    messageReceived += 1;
                            } else {
                                messageReceived += 1;
                            }
                        } else {
                            System.out.println(result);
                        }
                    }
                } else if (shouldTryConnection){
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
