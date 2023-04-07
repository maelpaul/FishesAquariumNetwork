package ProjetPoisson.project.threads;

import ProjetPoisson.mightylib.resources.Resources;
import ProjetPoisson.project.client.ClientTcp;
import ProjetPoisson.project.client.Configuration;
import ProjetPoisson.project.client.ServerTcp;

import java.io.*;
import java.net.Socket;
import java.net.SocketTimeoutException;


public  abstract class CommunicationThread implements Runnable {
    private volatile boolean running;
    protected String receivedMessage;
    protected String messageToSend;
    public void run() {
    }

    public boolean didReceiveMessage() {
    }

    public String message() {
    }

    public void sendMessage(String message) {
    }

    public boolean didProcessedMessage() {
    }

    public void doStop() {
    }


}
