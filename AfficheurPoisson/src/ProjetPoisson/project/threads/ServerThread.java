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
        private final CountDownLatch latch;
        private final CountDownLatch serverLatch;
        private volatile boolean serverConnected = false;

        public ServerThread(CountDownLatch latch, CountDownLatch serverLatch) {
            this.running = true;this.latch = latch;listening = true;this.serverLatch = serverLatch;
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
            server.acceptConnexion();
            serverConnected = true;
            serverLatch.countDown();

        listening = true;
        latch.countDown();
        System.out.println("[Debug] EIUHIUEHIUFHZIUFHIEZ");
        while (!serverConnected) {
            try {
                Thread.sleep(100);
            } catch (InterruptedException e) {
                e.printStackTrace();
            }
        }
        System.out.println("[Debug] Server Connected.");

        while (!server.hasClient()) {
            try {
                Thread.sleep(100);
            } catch (InterruptedException e) {
                e.printStackTrace();
            }
        }

        server.sendMessage("SHEESH!");
        System.out.println("[Debug] Server: Message sent.");

        try {
            TimeUnit.SECONDS.sleep(5);
        } catch (InterruptedException e) {
            e.printStackTrace();
        }

        while (running) {
            String message = server.readMessage();
            if (message != null) {
                System.out.println("Server: " + message);
                running = false;
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
