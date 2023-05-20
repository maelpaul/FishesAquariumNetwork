package ProjetPoisson.project.display;

import ProjetPoisson.mightylib.main.WindowInfo;
import ProjetPoisson.mightylib.resources.Resources;
import ProjetPoisson.mightylib.resources.texture.Texture;
import ProjetPoisson.project.client.Configuration;
import org.joml.Vector2f;

import java.io.File;
import java.util.*;
import java.util.Random;

public class FishManager {
    public enum EResult {
        AddSuccessfully,
        AddErrorNameExisting,

        AddErrorUnknownBehaviour,

        DeleteSuccessfully,
        DeleteErrorNameNotExisting,
    }

    private final static int UPDATE_FISH_COMMAND_SIZE = 3;
    private final static int UPDATE_FISH_ARG_NAME = 0;
    private final static int UPDATE_FISH_ARG_AT = 1;
    private final static int UPDATE_FISH_ARG_DATA = 2;


    private final static int UPDATE_FISH_ARG_SIZE = 1;
    private final static int UPDATE_FISH_ARG_WIDTH = 0;
    private final static int UPDATE_FISH_ARG_HEIGHT = 1;

    private final static int UPDATE_FISH_ARG_POSITION = 0;
    private final static int UPDATE_FISH_ARG_X = 0;
    private final static int UPDATE_FISH_ARG_Y = 1;

    private final static int UPDATE_FISH_ARG_TIME = 2;

    private final HashMap<String, Fish> fishes;
    ArrayList<String> fishesFileName = new ArrayList<>();
    private final Random rand = new Random();

    private final WindowInfo info;

    public FishManager(WindowInfo info, Configuration configuration){
        this.info = info;

        fishes = new HashMap<>();

        File folderFish = new File(configuration.getPathForFishesResources());

        String[] fileList = folderFish.list();

        if (fileList != null) {
            for (String childPath : fileList) {
                String fileName = childPath.substring(childPath.lastIndexOf("/") + 1, childPath.lastIndexOf("."));
                Resources.getInstance().createAndInit(Texture.class, fileName, configuration.getPathForFishesResources() + "/" + childPath);
                fishesFileName.add(fileName);
            }
        }
    }

    public EResult addFish(String name, Vector2f positionPercentage, Vector2f sizePercentage, String behaviour){
        if (fishes.containsKey(name))
            return EResult.AddErrorNameExisting;

        Fish.EFishServerBehaviour current = null;

        for (Fish.EFishServerBehaviour value : Fish.EFishServerBehaviour.values()){
            if (value.name().equals(behaviour)) {
                current = value;
                break;
            }
        }

        if (current == null)
            return EResult.AddErrorUnknownBehaviour;

        // Part to choose texture based on fish's name
        String fishTextureName = name;
        if (name.contains("_")){
            fishTextureName = name.substring(0, name.indexOf("_"));
        }

        boolean found = false;
        for (int i = 0; i < fishesFileName.size() && !found; ++i){
            if (fishesFileName.get(i).equalsIgnoreCase(fishTextureName)){
                fishTextureName = fishesFileName.get(i);
                found = true;
            }
        }

        if (!found)
            fishTextureName = fishesFileName.get(0);

        fishes.put(
                name,
                new Fish(info, name, fishTextureName , positionPercentage, sizePercentage)
        );

        return EResult.AddSuccessfully;
    }

    public EResult delFish(String name){
        if (!fishes.containsKey(name))
            return EResult.DeleteErrorNameNotExisting;

        fishes.get(name).unload();
        fishes.remove(name);

        return EResult.DeleteSuccessfully;
    }

    public void processFishUpdate(String fishArgs){
        System.out.println("Update Fish : " + fishArgs);

        String[] parts = fishArgs.trim().split(" ");
        if (parts.length != UPDATE_FISH_COMMAND_SIZE) {
            System.err.println("Cant analyse : " + fishArgs);

            return;
        }

        String data = parts[UPDATE_FISH_ARG_DATA].replace(" ", "");

        String[] dataParts = data.split(",");

        String size = dataParts[UPDATE_FISH_ARG_SIZE];

        float width = Float.parseFloat(size.split("x")[UPDATE_FISH_ARG_WIDTH]) / 100f;
        float height = Float.parseFloat(size.split("x")[UPDATE_FISH_ARG_HEIGHT]) / 100f;

        String position = dataParts[UPDATE_FISH_ARG_POSITION];

        float x = Float.parseFloat(position.split("x")[UPDATE_FISH_ARG_X]) / 100f;
        float y = Float.parseFloat(position.split("x")[UPDATE_FISH_ARG_Y]) / 100f;

        int time = Integer.parseInt(dataParts[UPDATE_FISH_ARG_TIME]);

        if (fishes.containsKey(parts[UPDATE_FISH_ARG_NAME])){
            fishes.get(parts[UPDATE_FISH_ARG_NAME]).    travelToNewPosition(
                    new Vector2f(x, y),
                    time
            );
        } else {
            EResult result = addFish(parts[UPDATE_FISH_ARG_NAME],
                    new Vector2f(x, y),
                    new Vector2f(width, height),
                    Fish.EFishServerBehaviour.values()[0].name());

            if (result != EResult.AddSuccessfully){
                System.err.println("Error when adding fish : " + result.name());
            }
        }
    }


    public String collectionToStr(String delimiter, int namePerLine, String lineDelimiter, Collection<String> c){
        StringBuilder result = new StringBuilder();
        int counter = 0;
        int gCounter = 0;

        for (String name : c){
            counter += 1;
            gCounter += 1;
            if (gCounter == c.size()) {
                result.append(name);
            } else if (counter == namePerLine) {
                result.append(name).append(lineDelimiter);
                counter = 0;
            } else {
                result.append(name).append(delimiter);
            }
        }

        return result.toString();
    }

    public String[] getFishNames (){
        return fishes.keySet().toArray(new String[0]);
    }

    public String getNamesStr(String delimiter, int namePerLine, String lineDelimiter){
        if (fishes.size() == 0)
            return null;

        return collectionToStr(delimiter, namePerLine, lineDelimiter, fishes.keySet());
    }

    public String getMovementsTypeStr(String delimiter, int namePerLine, String lineDelimiter){
        return collectionToStr(delimiter, namePerLine, lineDelimiter, Fish.GetFishBehaviourList());
    }

    public void update(){
        for (Fish fish : fishes.values()){
            fish.update();
        }
    }

    public void setShowName(boolean value){
        for (Fish fish : fishes.values()){
            fish.setShowName(value);
        }
    }

    public void display(){
        for (String name : fishes.keySet()){
            fishes.get(name).display();
        }
    }

    public void clear(){
        for (String name : fishes.keySet()){
            fishes.get(name).unload();
        }

        fishes.clear();
    }

    public void unload(){
        clear();
    }

    @Override
    public String toString(){
        return "";
    }
}
