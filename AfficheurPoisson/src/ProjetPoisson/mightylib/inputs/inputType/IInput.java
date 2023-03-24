package ProjetPoisson.mightylib.inputs.inputType;

import ProjetPoisson.mightylib.inputs.InputManager;

public interface IInput {
    boolean getState(InputManager inputManager);

    boolean inputPressed(InputManager inputManager);

    boolean inputReleased(InputManager inputManager);
}
