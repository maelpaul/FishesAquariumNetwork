package ProjetPoisson.project.threads;

import ProjetPoisson.mightylib.resources.Resources;
import ProjetPoisson.project.client.ClientTcp;
import ProjetPoisson.project.client.Configuration;

import java.io.*;
import java.net.Socket;
import java.net.SocketTimeoutException;
public class ClientThread extends CommunicationThread{
    protected ClientTcp client;
    public ClientThread() {
        this.running = true;
    }
    public void ClientSetup(){
        Configuration conf_client = Resources.getInstance().getResource(Configuration.class, "client");
        ClientTcp client = new ClientTcp(conf_client);
        client.tryCreateConnection();
        this.client = client;
    }

    public ClientTcp getClient(){
        return this.client;
    }
    public void run() {
        while(running){
            //System.out.println("HAHAHA2");
            /*
            client.sendMessage("CLIENT SHEESH");
            if (didReceiveMessage()){
                System.out.println("MESSAGE RECEIVED" + receivedMessage);
            }
             */
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
