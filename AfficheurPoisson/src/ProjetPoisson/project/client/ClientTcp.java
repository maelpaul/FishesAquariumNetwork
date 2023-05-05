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

    public ClientTcp(Configuration configuration){
        this.configuration = configuration;
    }

    private final static Charset ENCODING = StandardCharsets.UTF_8;

    public void tryCreateConnection() {
        try {
            socket = new Socket(configuration.getAddress(), configuration.getPort());
            System.out.println("[Debug] Socket created successfully."); // Add this line
        } catch (IOException e) {
            socket = null;
            System.out.println("Can't create socket with address : " + configuration.getAddress()
                    + " and port : " + configuration.getPort());
        }
    }


    public void sendMessage(String message) {
        try {
            if (socket != null && socket.isConnected()) {
                OutputStream os = socket.getOutputStream();
                byte[] dataBytes = message.getBytes(StandardCharsets.UTF_8);
                int dataLength = dataBytes.length;
                os.write(ByteBuffer.allocate(4).putInt(dataLength).array());
                os.write(dataBytes);
                os.flush();
                System.out.println("[Debug] Client sent message: " + message);
            } else {
                System.out.println("Unable to send message: Socket is not connected or is closed.");
            }
        } catch (IOException e) {
            e.printStackTrace();
        }
    }
    private int readMessageLength(InputStream stream) throws IOException {
        byte[] lengthBytes = new byte[4];
        int bytesRead = stream.read(lengthBytes);
        if (bytesRead != 4) {
            throw new IOException("Unable to read the message length.");
        }
        return ByteBuffer.wrap(lengthBytes).getInt();
    }



    public boolean isConnected() {
        return socket != null && socket.isConnected();
    }




    public String readMessage() {
        if (socket == null) {
            System.out.println("Can't read message while connection not initialized");
            return null;
        }

        InputStream stream = null;
        String message = null;

        try {
            stream = socket.getInputStream();
            byte[] byteMessage = new byte[1024];
            int bytesRead = stream.read(byteMessage);
            if (bytesRead == -1) {
                throw new IOException("End of stream reached before message was fully received.");
            }
            message = new String(byteMessage, 0, bytesRead, ENCODING);
        } catch (IOException e) {
            throw new RuntimeException(e);
        }

        return message;
    }


    public void connect() {
        try {
            //System.out.println(this.configuration);
            this.socket = new Socket(configuration.getAddress(), configuration.getPort());
            System.out.println("[Debug] Client TCP: Connection made successfully.");
        } catch (IOException e) {
            e.printStackTrace();
        }
    }



    public void closeConnection() {
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
                e.printStackTrace();
            } else {
                socket = null;
                System.out.println("Error while closing connection\n");
                e.printStackTrace();
            }
        }
    }

}
