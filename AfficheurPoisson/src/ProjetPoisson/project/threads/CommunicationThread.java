package ProjetPoisson.project.threads;

import ProjetPoisson.project.client.Message;

import java.util.ArrayList;
import java.util.HashMap;



public  abstract class CommunicationThread extends Thread {
    private int messageIdCounter;

    protected int processedMessageNumber;
    protected int receivedMessageNumber;

    protected volatile boolean running;
    protected ArrayList<Message> receivedMessages;

    protected HashMap<Integer, String> messageToSend;

    CommunicationThread(){
        receivedMessages = new ArrayList<>();
        messageToSend = new HashMap<>();
    }

    public void setup(){
        messageIdCounter = 0;
        processedMessageNumber = 0;
        receivedMessageNumber = 0;
    }

    public void reset(){
        messageIdCounter = 0;
        processedMessageNumber = 0;
        receivedMessageNumber = 0;

        receivedMessages.clear();
        messageToSend.clear();
    }

    public void run() {}

    public boolean didReceiveMessage() {
        return receivedMessages.size() != 0;
    }

    public final Message[] message() {
        Message[] result = new Message[receivedMessages.size()];
        receivedMessages.toArray(result);
        receivedMessages.clear();

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
