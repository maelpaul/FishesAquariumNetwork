package ProjetPoisson.project.threads;

import ProjetPoisson.mightylib.resources.Resources;
import ProjetPoisson.project.client.ClientTcp;
import ProjetPoisson.project.client.Configuration;
import ProjetPoisson.project.client.ClientTcp;

import java.io.*;
import java.net.Socket;
import java.net.SocketTimeoutException;
public class ClientThread {
    protected ClientTcp client;
    public void ClientThread() {
        this.running = true;
    }
    public void ServerSetup(){
        Configuration conf_client = Resources.getInstance().getResource(Configuration.class, "client");
        ClientTcp client = new ClientTcp(conf_client);
        client.tryCreateConnection();
        client.tryCreateConnection();
        this.client = client;
    }

    public ClientTcp getClient(){
        return this.client;
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
