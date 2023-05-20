package ProjetPoisson.project.command;

import java.util.ArrayList;

public class ResultCommand <T> {
    public final class Action{
        private final T action;
        private final Object[] args;

        public Action (T action, Object[] args){
            this.action = action;
            this.args = args;
        }

        public T getAction() { return action; }

        public boolean argsNull() { return  args == null; }
        public int argsSize() { return args == null ? 0 : args.length; }
        public Object getArgs(int i) { return args[i]; }
    }

    private final ArrayList<Action> actions;

    public ResultCommand(){
        actions = new ArrayList<>();
    }

    public ResultCommand<T> addAction(T action, Object[] args){
        actions.add(new Action(action, args));

        return this;
    }

    public Iterable<Action> getActions() {
        return actions;
    }
}
