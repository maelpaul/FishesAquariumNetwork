package ProjetPoisson.project.command;

public interface ICommand<T> {
    ResultCommand<T> process(String[] args);
    ResultCommand<T> returnHelp();
}