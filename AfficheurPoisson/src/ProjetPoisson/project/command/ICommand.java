package ProjetPoisson.project.command;

public interface ICommand {
    String process(String[] args);
    String returnHelp();
}