package ProjetPoisson.project.client;

public class Message {
    private final int id;
    private final String message;

    public Message (int id, String message){
        this.id = id;
        this.message = message;
    }

    public String getMessage() { return message; }
    public int getId() { return id; }
}
