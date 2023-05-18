package ProjetPoisson.project.command;

public interface ICommand {
    ResultCommand process(String[] args);
    ResultCommand returnHelp();
}