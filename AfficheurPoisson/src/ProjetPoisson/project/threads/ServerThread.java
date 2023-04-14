package ProjetPoisson.project.threads;
import java.util.concurrent.TimeUnit;
import ProjetPoisson.project.client.ServerTcp;
import ProjetPoisson.mightylib.resources.Resources;
import ProjetPoisson.project.client.ClientTcp;
import ProjetPoisson.project.client.Configuration;
import ProjetPoisson.project.client.ServerTcp;

import java.io.*;
import java.net.Socket;
import java.net.SocketTimeoutException;
import java.util.concurrent.CountDownLatch;

public class ServerThread extends CommunicationThread{
        private ServerTcp server;
        private volatile boolean listening = false;
        private volatile boolean serverConnected = false;

        public ServerThread() {
            this.running = true;listening = true;
        }
        public void ServerSetup(){
            System.out.println("[Debug] Setting up the server...");
            Configuration conf_server = Resources.getInstance().getResource(Configuration.class, "server");
            server = new ServerTcp(conf_server);
            server.tryCreateConnection();
        }
        public boolean isListening() {
            return listening;
        }
    public void run() {
        Socket socket = null;
        try {
            socket = new Socket("localhost", 8888);
        }
        catch (IOException e) {
            e.printStackTrace();
        }
        System.out.println("Connected to server");
        while(running && socket != null) {
            try {
                PrintWriter out = new PrintWriter(socket.getOutputStream(), true);
                out.println("Hello from client");

                BufferedReader in = new BufferedReader(new InputStreamReader(socket.getInputStream()));
                String response = in.readLine();
                System.out.println("Received response from server: " + response);
                socket.close();
                System.out.println("Disconnected from server");
                break;
            }
            catch (IOException e) {
                e.printStackTrace();
            }
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
        if (server != null && server.hasClient()) {
            server.closeConnexion();
        }
    }



}
