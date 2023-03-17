package ProjetPoisson.mightylib.inputs.keyboardlanguage;

public class NativeKeyboardLanguage implements KeyboardLanguage {
    private static final KeyboardLanguage instance = new NativeKeyboardLanguage();

    public static KeyboardLanguage getInstance() { return instance; }
    private NativeKeyboardLanguage(){}

    public int translateKeyTo(int keyId){
        return keyId;
    }

    @Override
    public String keyboardConfigurationName() {
        return "qwerty";
    }
}
