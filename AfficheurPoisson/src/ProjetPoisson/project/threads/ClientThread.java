package ProjetPoisson.project.threads;
import java.util.concurrent.CountDownLatch;
import java.util.concurrent.TimeUnit;
import ProjetPoisson.mightylib.resources.Resources;
import ProjetPoisson.project.client.ClientTcp;
import ProjetPoisson.project.client.Configuration;

import java.io.*;
import java.net.Socket;
import java.net.SocketTimeoutException;
public class ClientThread extends CommunicationThread {
    protected ClientTcp client;
    private final CountDownLatch latch;
    private boolean readyToSend = false;

    public ClientThread(CountDownLatch latch) {
        this.running = true;
        this.latch = latch;
    }

    public void ClientSetup() {
        System.out.println("[Debug] Setting up the client...");
        Configuration conf_client = Resources.getInstance().getResource(Configuration.class, "client");
        client = new ClientTcp(conf_client);
        client.tryCreateConnection();
        client.connect();
    }


    public ClientTcp getClient() {
        return this.client;
    }

    public void run() {
        try {
            latch.await();
        } catch (InterruptedException e) {
            e.printStackTrace();
        }

        while (client == null || !client.isConnected()) {
            try {
                client.connect();
                Thread.sleep(100);
            } catch (InterruptedException e) {
                e.printStackTrace();
            }
        }
        System.out.println("[Debug] Client: Connected.");

        try {
            TimeUnit.SECONDS.sleep(1);
        } catch (InterruptedException e) {
            e.printStackTrace();
        }

        while (running) {
            String message = client.readMessage();
            if (message != null) {
                System.out.println("Client: " + message);
                running = false;
            }
        }
        doStop();
    }






    public void setReadyToSend(boolean ready) {
        this.readyToSend = ready;
    }


    public boolean didReceiveMessage() {
        return receivedMessage != null;
    }

    public String message() {
        if (receivedMessage != null) {
            String tempMessage = receivedMessage;
            receivedMessage = null;
            return tempMessage;
        }
        return null;
    }

    public void sendMessage(String message) {
        messageToSend = message;
    }

    public void doStop() {
        this.running = false;
        if (client != null && client.isConnected()) {
            client.closeConnection();
        }
    }
}
