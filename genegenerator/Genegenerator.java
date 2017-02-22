/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package genegenerator;

import java.io.BufferedWriter;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.OutputStreamWriter;
import java.io.Writer;
import java.util.Random;

/**
 *
 * @author luke
 */
public class Genegenerator {

    /**
     * @param args the command line arguments
     */
    static String family;
    static Random rng = new Random();
    static int lmerlength = 100;
    static int numchar = 40000;
    static int maxdist = 3000;
    
    public static void main(String[] args) throws IOException {
        for(int i=0;i<lmerlength;i++)
        {
            family+=randomChar(4);
        }
        try (Writer writer = new BufferedWriter(new OutputStreamWriter(
                new FileOutputStream("testgene.txt"), "utf-8"))) {
            int c = 0;
            int timetilstart = rng.nextInt(maxdist);
            int i = 0;
            while (timetilstart>0 || i < numchar) {
                if (i % 40 == 0) {
                    writer.write("\n");
                }
                i++;
                if (timetilstart == 0) {
                    if (rng.nextInt(100)==1) {
                        writer.write(randomChar(8));
                    } else {
                        writer.write(family.charAt(c));
                    }
                    c++;
                    if (c >= family.length()) {
                        c = 0;
                        timetilstart = rng.nextInt(maxdist);
                    }
                } else {
                    char next = randomChar(8);
                    writer.write(next);
                    timetilstart--;
                }
            }
        }
    }

    private static char randomChar(int numrand) {
        int next = rng.nextInt(numrand);
        switch (next) {
            case 0:
                return 'a';
            case 1:
                return 'c';
            case 2:
                return 'g';
            case 3:
                return 't';
            case 4:
                return 'a';
            case 5:
                return 'c';
            case 6:
                return 'g';
            case 7:
                return 't';
            case 8:
                return 'n';
        }
        return 'a';
    }
}
