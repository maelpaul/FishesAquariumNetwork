package ProjetPoisson.project.command;

public class PromptResultCommand extends ResultCommand<String> {

    public PromptResultCommand(String text){
        super();

        addAction("resultPrompt", new Object[]{text});
    }

    public PromptResultCommand(String text, String breakLineDelimiter){
        super();

        addAction("resultPrompt", new Object[]{text, breakLineDelimiter});
    }

}
