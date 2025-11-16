import java.io.BufferedReader;
import java.io.InputStreamReader;
import java.io.IOException;

public class java {
    public static void main(String[] args) throws IOException {
        BufferedReader reader = new BufferedReader(new InputStreamReader(System.in));
        String line = reader.readLine();
        if (line != null) {
            System.out.println(line);
        }
        reader.close();
    }
}
