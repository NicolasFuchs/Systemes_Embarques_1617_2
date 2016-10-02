public class Main {

    public static final long LOOPS = 8;

    public static void main(String[] args) {
        long res = 16;
        short incr = 32;
        long i = 0;
        while(i != LOOPS) {
            res += incr;
            i++;
        }
    }
}
