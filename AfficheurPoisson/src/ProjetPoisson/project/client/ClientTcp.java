package ProjetPoisson.project.client;
import java.io.*;
import java.net.*;
import java.nio.ByteBuffer;
import java.nio.charset.Charset;
import java.nio.charset.StandardCharsets;
import java.util.Base64;

public class ClientTcp {
    private final Configuration configuration;
    private Socket socket;

    private long lastHeartbeat;
    private static final int HEARTBEAT_INTERVAL_MS = 10000;
    private static final int HEARTBEAT_TIMEOUT_MS = 15000;

    private volatile boolean running = true;

    private boolean TryingConnection;

    public ClientTcp(Configuration configuration){
        this.configuration = configuration;
    }

    private final static Charset ENCODING = StandardCharsets.UTF_8;

    public void tryCreateConnection() {
        try {
            TryingConnection = true;
            socket = new Socket(configuration.getAddress(), configuration.getPort());
            //System.out.println("[Debug] Socket created successfully.");
            TryingConnection = false;
        } catch (IOException e) {
            TryingConnection = false;
            socket = null;
            System.out.println("Can't create socket with address : " + configuration.getAddress()
                    + " and port : " + configuration.getPort());
        }
    }

    public boolean isTryingConnection() {
        return TryingConnection;
    }

    public void sendMessage(String message) {
        try {
            if (socket != null && socket.isConnected()) {
                OutputStream outputStream = socket.getOutputStream();
                outputStream.write(message.getBytes());
                outputStream.flush();
                //System.out.println("[Debug] Client sent message: " + message);
            } else {
                System.out.println("Unable to send message: Socket is not connected or is closed.");
            }
        } catch (IOException e) {
            e.printStackTrace();
        }
    }


    public boolean isConnected() {
        return socket != null && socket.isConnected();
    }

    public String readMessage() {
        if (socket == null || !socket.isConnected()) {
            System.out.println("Can't read message while connection not initialized or disconnected");
            return null;
        }

        InputStream stream = null;
        String message = null;

        try {
            stream = socket.getInputStream();
            if (stream.available() > 0) {
                byte[] byteMessage = new byte[1024];
                int bytesRead = stream.read(byteMessage);

                if (bytesRead == -1) {
                    throw new IOException("End of stream reached before message was fully received.");
                }

                message = new String(byteMessage, 0, bytesRead, ENCODING);
                lastHeartbeat = System.currentTimeMillis(); // Update heartbeat when message is successfully read
            } else {
                return null;
            }

        } catch (EOFException | SocketException e) {
            System.out.println("Connection lost with the server. (EOF or SocketException)");
            closeConnection();
            return "Connection lost with the server.";
        } catch (SocketTimeoutException e) {
            System.out.println("Socket read timed out. Connection may be lost.");
            closeConnection(); // Close connection on timeout
            return "Connection lost with the server.";
        } catch (IOException e) {
            System.out.println("An error occurred while reading message from the server.");
            e.printStackTrace();
        }

        return message;
    }

    private void heartbeatLoop() {
        while (running) {
            if (System.currentTimeMillis() - lastHeartbeat > HEARTBEAT_TIMEOUT_MS) {
                System.out.println("Heartbeat timeout. Closing connection.");
                closeConnection();
            }

            try {
                Thread.sleep(HEARTBEAT_INTERVAL_MS);
            } catch (InterruptedException e) {
                e.printStackTrace();
            }
        }
    }

    public void connect() {
        try {
            this.socket = new Socket(configuration.getAddress(), configuration.getPort());
            this.socket.setSoTimeout(HEARTBEAT_INTERVAL_MS);
            lastHeartbeat = System.currentTimeMillis(); // Initialize lastHeartbeat at the time of connection
            new Thread(this::heartbeatLoop).start();
            startReading();
        } catch (IOException e) {
            e.printStackTrace();
        }
    }




    public void closeConnection() {
        running = false; // Stop the heartbeat loop
        if (socket == null) {
            System.out.println("Can't close uncreated connection.");
            return;
        }

        try {
            socket.shutdownInput();
            socket.shutdownOutput();
            socket.close();
        } catch (IOException e) {
            if (e instanceof SocketException && e.getMessage().equals("Socket is closed")) {
                System.out.println("Socket was already closed.");
            } else {
                socket = null;
                System.out.println("Error while closing connection\n");
                e.printStackTrace();
            }
        }
    }


    public void startReading() {
        new Thread(() -> {
            while (running) { // Only read messages while running
                //System.out.println("HAHAHA");
                readMessage();
            }
        }).start();
    }
}
