package ProjetPoisson.project.lib;
import ProjetPoisson.mightylib.inputs.InputManager;

public class ActionId {
    // Terminal
    public static final int MENU_PREVIOUS_COMMAND = InputManager.getAndIncrementId();
    public static final int MENU_NEXT_COMMAND = InputManager.getAndIncrementId();
    public static final int TEXT_INPUT_LEFT_DELETE = InputManager.getAndIncrementId();
    public static final int TEXT_INPUT_RIGHT_DELETE = InputManager.getAndIncrementId();

    public static final int TEXT_CURSOR_LEFT = InputManager.getAndIncrementId();
    public static final int TEXT_CURSOR_RIGHT = InputManager.getAndIncrementId();

    public static final int SHIFT_MODIFIER = InputManager.getAndIncrementId();
    public static final int ALT_MODIFIER = InputManager.getAndIncrementId();
    public static final int ENTER_COMMAND = InputManager.getAndIncrementId();
    public static final int TEXT_INPUT_PASTE = InputManager.getAndIncrementId();

    // Command

    public static final int SHOW_HIDE_TEXTS = InputManager.getAndIncrementId();
}
