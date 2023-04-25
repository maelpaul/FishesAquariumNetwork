package ProjetPoisson.project.display;

import ProjetPoisson.mightylib.main.WindowInfo;
import ProjetPoisson.mightylib.resources.Resources;
import ProjetPoisson.mightylib.resources.texture.Texture;
import ProjetPoisson.project.client.Configuration;
import org.joml.Vector2f;

import java.io.File;
import java.util.*;

public class FishManager {
    public enum EResult {
        AddSuccessfully,
        AddErrorNameExisting,

        DeleteSuccessfully,
        DeleteErrorNameNotExisting
    }

    private final HashMap<String, Fish> fishes;
    private final ArrayList<String> movementType;
    ArrayList<String> fishesFileName = new ArrayList<>();
    private final Random rand = new Random();

    private final WindowInfo info;

    public FishManager(WindowInfo info, Configuration configuration){
        this.info = info;

        fishes = new HashMap<>();
        movementType = new ArrayList<>();
        movementType.add("guided");

        File folderFish = new File(configuration.getPathForFishesResources());
        for (String childPath : Objects.requireNonNull(folderFish.list())) {
            String fileName = childPath.substring(childPath.lastIndexOf("/") + 1, childPath.lastIndexOf("."));
            Resources.getInstance().createAndInit(Texture.class, fileName, configuration.getPathForFishesResources() + "/" + childPath);
            fishesFileName.add(fileName);
        }
    }

    public EResult addFish(String name, Vector2f positionPercentage, Vector2f sizePourcentage, String behaviour){
        if (fishes.containsKey(name))
            return EResult.AddErrorNameExisting;

        fishes.put(
                name,
                new Fish(info,  fishesFileName.get(0), positionPercentage, sizePourcentage)
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

    public String getName (String delimiter, int namePerLine, String lineDelimiter){
        if (fishes.size() == 0)
            return null;

        return collectionToStr(delimiter, namePerLine, lineDelimiter, fishes.keySet());
    }

    public String getMovementType (String delimiter, int namePerLine, String lineDelimiter){
        if (movementType.size() == 0)
            return null;

        return collectionToStr(delimiter, namePerLine, lineDelimiter, movementType);
    }

    public void update(){
        for (Fish fish : fishes.values()){
            fish.update();

            if (fish.finishedTravel()){
                Vector2f position = new Vector2f(rand.nextFloat(), rand.nextFloat());
                fish.travelToNewPosition(position, 1 + rand.nextFloat() * 3);
            }
        }
    }

    public void display(){
        for (String name : fishes.keySet()){
            fishes.get(name).display();
        }
    }

    public void unload(){
        for (String name : fishes.keySet()){
            fishes.get(name).unload();
        }

        fishes.clear();
    }

    @Override
    public String toString(){
        return "";
    }
}