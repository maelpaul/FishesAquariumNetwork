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
        try {
            Socket socket = new Socket("localhost", 8888);
            System.out.println("Connected to server");

            PrintWriter out = new PrintWriter(socket.getOutputStream(), true);
            out.println("Hello from client");

            BufferedReader in = new BufferedReader(new InputStreamReader(socket.getInputStream()));
            String response = in.readLine();
            System.out.println("Received response from server: " + response);

            socket.close();
            System.out.println("Disconnected from server");
        } catch (IOException e) {
            e.printStackTrace();
        }
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
