package ProjetPoisson.project.threads;

import ProjetPoisson.project.client.Message;

import java.util.ArrayList;
import java.util.HashMap;



public  abstract class CommunicationThread extends Thread {
    private int messageIdCounter;

    protected int messageProcessed;
    protected int messageReceived;

    protected volatile boolean running;
    protected ArrayList<Message> receivedMessages;

    protected HashMap<Integer, String> messageToSend;

    CommunicationThread(){
        receivedMessages = new ArrayList<>();
        messageToSend = new HashMap<>();

        messageIdCounter = 0;
        messageProcessed = 0;
        messageReceived = 0;
    }

    public void run() {}

    public boolean didReceiveMessage() {
        return receivedMessages.size() == 0;
    }

    public final Message[] message() {
        Message[] result = new Message[receivedMessages.size()];
        receivedMessages.toArray(result);
        return result;

    }

    public final int sendMessage(String message) {
        messageToSend.put(messageIdCounter, message);
        return messageIdCounter++;
    }

    public final boolean didProcessedMessage() {
        return false;
    }

    public abstract void doStop();
}
