package ProjetPoisson.mightylib.inputs.keyboardlanguage;

public class AZERTYKeyboardLanguage implements KeyboardLanguage {
    private static final KeyboardLanguage instance = new AZERTYKeyboardLanguage();

    public static KeyboardLanguage getInstance() { return instance; }
    @Override
    public int translateKeyTo(int keyId) {
        switch (keyId){

        }

        return keyId;
    }

    @Override
    public String keyboardConfigurationName() {
        return "azerty";
    }
}
