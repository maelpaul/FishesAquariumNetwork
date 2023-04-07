package ProjetPoisson.project.client;

import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.net.ServerSocket;
import java.net.Socket;
import java.nio.charset.Charset;
import java.nio.charset.StandardCharsets;
import java.util.Base64;

public class ServerTcp {
    private final Configuration configuration;
    private ServerSocket serverSocket;
    private Socket client;

    public ServerTcp(Configuration configuration){
        this.configuration = configuration;
    }

    private final static Charset ENCODING = StandardCharsets.UTF_8;

    public void tryCreateConnection(){
        try {
            serverSocket = new ServerSocket(configuration.getPort());
        } catch (IOException e) {
            serverSocket = null;
            System.out.println("Can't create socket with port : " + configuration.getPort());
            e.printStackTrace();
        }
    }


    public void acceptConnexion(){
        try {
            client = serverSocket.accept();
        } catch (IOException e) {
            throw new RuntimeException(e);
        }
    }

    public void sendMessage(String data){
        if (client == null){
            System.out.println("Can't send message while connection not initialized");
            return;
        }

        OutputStream output;

        try {
            output = client.getOutputStream();
            output.write(data.getBytes(ENCODING));

            output.close();
        } catch (IOException e) {
            throw new RuntimeException(e);
        }
    }

    public String readMessage(){
        if (client == null){
            System.out.println("Can't read message while connection not initialized");
            return null;
        }

        InputStream stream = null;
        String message = null;

        try {
            stream = client.getInputStream();
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

    public void closeConnexion(){
        if (client == null){
            System.out.println("Can't close uncreated connection.");
            return;
        }

        try {
            client.close();
            serverSocket.close();
        } catch (IOException e) {
            client = null;
            System.out.println("Error while closing connection\n");
            e.printStackTrace();
        }
    }
}
