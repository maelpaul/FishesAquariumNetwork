package ProjetPoisson.project.threads;

import ProjetPoisson.mightylib.resources.Resources;
import ProjetPoisson.project.client.ClientTcp;
import ProjetPoisson.project.client.Configuration;
import ProjetPoisson.project.client.ServerTcp;

import java.io.*;
import java.net.Socket;
import java.net.SocketTimeoutException;
import java.util.concurrent.CountDownLatch;



public  abstract class CommunicationThread implements Runnable {
    protected volatile boolean running;
    protected String receivedMessage;
    protected String messageToSend;
    public void run() {

    }

    public boolean didReceiveMessage() {
        return false;
    }

    public String message() {
        return null;
    }

    public void sendMessage(String message) {
    }

    public boolean didProcessedMessage() {
        return false;
    }

    public void doStop() {
    }

}
