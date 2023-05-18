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
        this.ServerSetup();
        System.out.println("Server Connected");
        server.acceptConnexion();
        System.out.println("Server: Client connected");

        String message = server.readMessage();
        System.out.println("Server: Received message from client: " + message);

        server.sendMessage("Hello from server");
        System.out.println("Server: Sent response to client: Hello from server");

        server.closeConnexion();
        System.out.println("Server: Client disconnected");
    }

    public void doStop() {
        this.running = false;
        if (server != null && server.hasClient()) {
            server.closeConnexion();
        }
    }



}
