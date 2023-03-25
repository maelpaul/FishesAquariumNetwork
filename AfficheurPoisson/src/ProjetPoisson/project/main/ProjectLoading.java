package ProjetPoisson.project.main;

import ProjetPoisson.mightylib.inputs.inputType.*;
import ProjetPoisson.mightylib.inputs.InputManager;
import ProjetPoisson.mightylib.main.Context;
import ProjetPoisson.mightylib.resources.Resources;
import ProjetPoisson.project.client.ConfigurationLoader;
import ProjetPoisson.project.lib.ActionId;
import org.lwjgl.glfw.GLFW;

import static org.lwjgl.glfw.GLFW.*;

class ProjectLoading {

    static void init(){
        Resources resources = Resources.getInstance();
        resources.Loaders.add(new ConfigurationLoader());

        addCommands();
    }

    static void addCommands(){

    }

    static void ContextLoading(Context context){
        InputManager inputManager = context.getInputManager();

        ActionInput[] configurations = {
                new ActionInput(ActionId.MENU_PREVIOUS_COMMAND, "MENU_PREVIOUS_COMMAND",
                        new InputSimple(GLFW_KEY_UP, EInputType.Keyboard)),

                new ActionInput(ActionId.MENU_NEXT_COMMAND, "MENU_NEXT_COMMAND",
                        new InputSimple(GLFW_KEY_DOWN, EInputType.Keyboard)),



                new ActionInput(ActionId.TEXT_INPUT_LEFT_DELETE, "TEXT_INPUT_LEFT_DELETE",
                        new InputSimple(GLFW_KEY_BACKSPACE, EInputType.Keyboard)),

                new ActionInput(ActionId.TEXT_INPUT_RIGHT_DELETE, "TEXT_INPUT_RIGHT_DELETE",
                        new InputOr(
                                new InputSimple(GLFW_KEY_DELETE, EInputType.Keyboard),
                                new InputAnd(
                                        new InputOr(
                                                new InputSimple(GLFW_KEY_LEFT_SHIFT, EInputType.Keyboard),
                                                new InputSimple(GLFW_KEY_RIGHT_SHIFT, EInputType.Keyboard)
                                        ),
                                        new InputSimple(GLFW.GLFW_KEY_KP_DECIMAL, EInputType.Keyboard)
                                ))),

                new ActionInput(ActionId.TEXT_CURSOR_LEFT, "TEXT_CURSOR_LEFT",
                        new InputSimple(GLFW_KEY_LEFT, EInputType.Keyboard)),

                new ActionInput(ActionId.TEXT_CURSOR_RIGHT, "TEXT_CURSOR_RIGHT",
                        new InputSimple(GLFW_KEY_RIGHT, EInputType.Keyboard)),



                new ActionInput(ActionId.SHIFT_MODIFIER, "SHIFT_MODIFIER",
                        new InputOr(
                                new InputSimple(GLFW_KEY_LEFT_SHIFT, EInputType.Keyboard),
                                new InputSimple(GLFW_KEY_RIGHT_SHIFT, EInputType.Keyboard)
                        )),

                new ActionInput(ActionId.ALT_MODIFIER, "ALT_MODIFIER",
                        new InputSimple(GLFW_KEY_LEFT_SHIFT, EInputType.Keyboard)),



                new ActionInput(ActionId.ENTER_COMMAND, "ENTER_COMMAND",
                        new InputOr(
                                new InputSimple(GLFW_KEY_ENTER, EInputType.Keyboard),
                                new InputSimple(GLFW_KEY_KP_ENTER, EInputType.Keyboard))
                        ),

                new ActionInput(ActionId.TEXT_INPUT_PASTE, "TEXT_INPUT_PASTE",
                        new InputOr(
                                new InputAnd(
                                        new InputSimple(GLFW_KEY_V, EInputType.Keyboard),
                                        new InputSimple(GLFW_KEY_LEFT_CONTROL, EInputType.Keyboard)
                                ),
                                new InputAnd(
                                        new InputSimple(GLFW_KEY_V, EInputType.Keyboard),
                                        new InputSimple(GLFW_KEY_RIGHT_CONTROL, EInputType.Keyboard)
                                )
                        ))
        };


        inputManager.init(configurations);
    }
}
