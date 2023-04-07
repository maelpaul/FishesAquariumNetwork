package ProjetPoisson.project.threads;

import ProjetPoisson.project.client.ServerTcp;
import ProjetPoisson.mightylib.resources.Resources;
import ProjetPoisson.project.client.ClientTcp;
import ProjetPoisson.project.client.Configuration;
import ProjetPoisson.project.client.ServerTcp;

import java.io.*;
import java.net.Socket;
import java.net.SocketTimeoutException;

public class ServerThread extends CommunicationThread{
        private ServerTcp server;
        public void ServerThread() {
            this.running = true;
        }
        public void ServerSetup(){
            Configuration conf_server = Resources.getInstance().getResource(Configuration.class, "server");
            ServerTcp server = new ServerTcp(conf_server);
            server.tryCreateConnection();
            server.acceptConnexion();
            this.server = server;
        }

        public ServerTcp getServer(){
            return this.server;
        }
        public void run() {
            while(running){
                server.sendMessage("SERVER SHEEESH");
                if (didReceiveMessage()){
                    System.out.println("MESSAGE RECEIVED" + receivedMessage);
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
        }


    }
