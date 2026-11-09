import java.util.Scanner;

public class WeakestLink {

    public static void main(String[] args) {
        
        int banked = 0;
        int collected = 0;
        int i = 0;
        int[] money = {0, 20, 50, 100, 200, 300, 450, 600, 1000};

        System.out.printf("Money in bank: %d kr \n\n", banked);
        Scanner scanner = new Scanner(System.in);

        while(true) {
            String input = scanner.nextLine();

            if(input.trim().equals("c")) {
                i++;
                int m;
                if(i > 8) {
                    m = money[8];
                    collected += money[8];
                } else {
                    m = money[i];
                    collected += money[i];
                }
                System.out.printf("Chain: %d \n", i);
                System.out.printf("Collected: %d kr \n", m);
                System.out.printf("Total this round: %d kr \n\n", collected);

            } else if(input.trim().equals("f")) {
                collected = 0;
                i = 0;
                System.out.println("You broke the chain!");
                System.out.printf("Money in bank: %d kr \n\n", banked);

            } else if(input.trim().equals("b")) {
                banked += collected;
                System.out.printf("You banked %d kr! \n", collected);
                System.out.printf("Money in bank: %d kr \n\n", banked);
                i = 0;
                collected = 0;
            } else {
                System.out.println("Something went wrong. Commands: c - correct, f - fail, b - bank\n\n");
            }
        }
    }
}