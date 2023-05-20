package ProjetPoisson.project.command;

public class PromptResultCommand extends ResultCommand<String> {

    public PromptResultCommand(String text){
        super();

        addPromptResultAction(text);
    }

    private void addPromptResultAction(String text){
        addAction("resultPrompt", new Object[]{text});
    }
}
