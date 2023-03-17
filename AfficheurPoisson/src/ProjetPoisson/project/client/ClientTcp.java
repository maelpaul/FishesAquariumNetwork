package ProjetPoisson.project.client;
import java.io.*;
import java.net.*;
import java.nio.charset.Charset;
import java.nio.charset.StandardCharsets;
import java.util.Base64;

public class ClientTcp {
    private Configuration configuration;
    private Socket socket;

    public ClientTcp(Configuration configuration){
        this.configuration = configuration;
    }

    private final static Charset ENCODING = StandardCharsets.UTF_8;

    public void tryCreateConnection(){
        try {
            socket = new Socket(configuration.getAddress(), configuration.getPort());
        } catch (IOException e) {
            socket = null;
            System.out.println("Can't create socket with adress : " + configuration.getAddress()
                + " and port : " + configuration.getPort());
        }
    }

    public void sendMessage(String data){
        if (socket == null){
            System.out.println("Can't send message while connection not initialized");
            return;
        }

        OutputStream output;

        try {
            output = socket.getOutputStream();
            output.write(data.getBytes(ENCODING));

            output.close();
        } catch (IOException e) {
            throw new RuntimeException(e);
        }
    }

    public String readMessage(){
        if (socket == null){
            System.out.println("Can't read message while connection not initialized");
            return null;
        }

        InputStream stream = null;
        String message = null;

        try {
            stream = socket.getInputStream();
        } catch (IOException e) {
            throw new RuntimeException(e);
        }

        try {
            byte[] byteMessage = stream.readAllBytes();

            message = Base64.getEncoder().encodeToString(byteMessage);
        } catch (IOException e) {
            throw new RuntimeException(e);
        }

        try {
            stream.close();
        } catch (IOException e) {
            throw new RuntimeException(e);
        }

        return message;
    }

    public void closeConnection(){
        if (socket == null){
            System.out.println("Can't close uncreated connection.");
            return;
        }

        try {
            socket.close();
        } catch (IOException e) {
            socket = null;
            System.out.println("Error while closing connection\n");
             e.printStackTrace();
        }
    }
}
