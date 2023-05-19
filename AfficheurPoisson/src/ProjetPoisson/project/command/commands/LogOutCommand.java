package ProjetPoisson.project.command.commands;

import ProjetPoisson.project.command.ICommand;
import ProjetPoisson.project.command.ResultCommand;

public class LogOutCommand implements ICommand  {

    @Override
    public ResultCommand process(String[] args) {
        if (args.length != 2 && !args[1].equals("out"))
            return new ResultCommand("  -> NOK : Mauvais argument(s), faites \"delFish help\" pour plus d'aide");

        return new ResultCommand("  -> OK : Log out", ResultCommand.EResultAction.SendServer);
    }

    @Override
    public ResultCommand returnHelp() {
        return new ResultCommand("  -> help(log out) : \n" +
                "    log out : disconnect the application to the server");
    }
}
