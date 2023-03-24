package ProjetPoisson.project.lib;
import ProjetPoisson.mightylib.inputs.InputManager;

public class ActionId {
    public static final int MENU_PREVIOUS_COMMAND = InputManager.getAndIncrementId();
    public static final int MENU_NEXT_COMMAND = InputManager.getAndIncrementId();
    public static final int TEXT_INPUT_LEFT_DELETE = InputManager.getAndIncrementId();
    public static final int TEXT_INPUT_RIGHT_DELETE = InputManager.getAndIncrementId();
    public static final int SHIFT_MODIFIER = InputManager.getAndIncrementId();
    public static final int ALT_MODIFIER = InputManager.getAndIncrementId();
    public static final int ENTER_COMMAND = InputManager.getAndIncrementId();
    public static final int TEXT_INPUT_PASTE = InputManager.getAndIncrementId();
}
