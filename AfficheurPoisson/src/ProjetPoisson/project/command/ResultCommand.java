package ProjetPoisson.project.command;

public final class ResultCommand {
    public enum EResultAction {
        None,
        Clear,
        SendServer,
        Quit,
    }

    private final String promptResult;
    private final EResultAction resultAction;

    public ResultCommand(String promptResult, EResultAction resultAction){
        this.promptResult = promptResult;
        this.resultAction = resultAction;
    }

    public ResultCommand(){
        this.promptResult = "";
        this.resultAction = EResultAction.None;
    }

    public ResultCommand(String promptResult){
        this.promptResult = promptResult;
        this.resultAction = EResultAction.None;
    }

    public ResultCommand(EResultAction resultAction){
        this.promptResult = "";
        this.resultAction = resultAction;
    }

    public String getPromptResult() { return promptResult; }
    public EResultAction getResultAction() { return resultAction; }
}
