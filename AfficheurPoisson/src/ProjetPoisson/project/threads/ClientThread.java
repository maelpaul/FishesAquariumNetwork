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

    public ClientThread() {
        this.running = true;
    }

    public void ClientSetup() {
        System.out.println("[Debug] Setting up the client...");
        Configuration conf_client = Resources.getInstance().getResource(Configuration.class, "client");
        client = new ClientTcp(conf_client);
        client.tryCreateConnection();
    }


    public ClientTcp getClient() {
        return this.client;
    }

    public void run() {
        this.ClientSetup();
        try {
            Thread.sleep(1000); // Add a delay of 500 milliseconds
        } catch (InterruptedException e) {
            e.printStackTrace();
        }

        client.sendMessage("addFish PoissonRouge at 90x40, 10x4, RandomWayPoint");

        String response = client.readMessage();
        System.out.println("Received response from server: " + response);
        client.sendMessage("Azy je me casse");
        //client.closeConnection();
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
