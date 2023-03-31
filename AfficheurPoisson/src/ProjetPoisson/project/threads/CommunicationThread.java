package ProjetPoisson.project.threads;

import java.io.*;
import java.net.Socket;
import java.net.SocketTimeoutException;


public class CommunicationThread implements Runnable {
    private volatile boolean running;
    private String receivedMessage;
    private String messageToSend;
    private boolean processedMessage;

    public CommunicationThread() {
        this.running = true;
    }

    public void run() {
        System.err.println("hello");
        while(true){

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

    public boolean didProcessedMessage() {
        if (processedMessage) {
            processedMessage = false;
            return true;
        }
        return false;
    }

    public void doStop() {
        this.running = false;
    }


}
