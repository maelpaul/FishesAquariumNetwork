package ProjetPoisson.project.main;

public class Main {

    public static boolean admin;

    /**
     * Run the game.
     */
    public static void main(String[] args) {
        admin = true;

        MainLoop mainLoop = new MainLoop();
        mainLoop.run();
    }
}
