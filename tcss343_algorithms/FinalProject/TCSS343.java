/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package tcss343;

import java.io.File;
import java.io.FileNotFoundException;
import java.io.FileWriter;
import java.io.IOException;
import java.util.ArrayList;
import java.util.Random;
import java.util.Scanner;
import java.util.logging.Level;
import java.util.logging.Logger;

/**
 * @author Jesse Bannon
 */
public class TCSS343
{
    static private ArrayList<String> matString = null;
    static private int mat[][] = null;
    static private int n;
    
    /**
     * @param args the command line arguments
     */
    public static void main(String[] args) throws IOException
    {
        
         if (args.length != 1)
         {
             System.out.println("Include path to matrix input file.");
         }
         else if (args[0].equalsIgnoreCase("test"))
         {
             try
             {
                 writeRandMatrix(26);
             }
             catch (IOException ex)
             {
                 Logger.getLogger(TCSS343.class.getName()).log(Level.SEVERE, null, ex);
             }
         }
         else if (!readMatrix(args[0]))
         {
             System.out.println("An error occured when loading the matrix input file.");
         }
         else
         {
             ArrayList<Integer> bruteForce = bruteForce();
             ArrayList<Integer> divideAndConquer = divideAndConquer();
             ArrayList<Integer> dynamicProgramming = dynamicProgramming();
             
             System.out.println("Jesse Bannon\nTCSS 343 - Algorithms\nProgramming Assignment\n");
             System.out.println("Matrix:");
             for (String line : matString)
             {
                 System.out.println(line);
             }
             
             System.out.println("\nBrute Force:");
             System.out.println(bruteForce.get(0) + "  -  " + bruteForce.subList(1, bruteForce.size()));
             
             System.out.println("\nDivide And Conquer:");
             System.out.println(divideAndConquer.get(0) + "  -  " + divideAndConquer.subList(1, divideAndConquer.size()));
             
             System.out.println("\nDynamic Programming:");
             System.out.println(dynamicProgramming.get(0) + "  -  " + dynamicProgramming.subList(1, dynamicProgramming.size()));
         }
    }
    
    static
    private boolean readMatrix(final String fileName)
    {
        final File inFile = new File(fileName);
        matString = new ArrayList<>();
        String lineValues[];
        int i = 0, j = 0;
        try
        {
            final Scanner sc = new Scanner(inFile);
            while (sc.hasNextLine())
            {
                matString.add(sc.nextLine());
            }
            
            n = matString.size();
            mat = new int[n][n];
            
            for (String line : matString)
            {
                lineValues = line.split("\t");
                for (String value : lineValues)
                {
                    mat[i][j] = value.trim().equalsIgnoreCase("NA") ? -1 : Integer.valueOf(value.trim());
                    ++j;
                }
                ++i;
                j = 0;
            }
            return true;
        }
        catch (FileNotFoundException ex)
        {
            return false;
        }
    }


    
    static
    public ArrayList<Integer> bruteForce()
    {
        final ArrayList<ArrayList<Integer>> powerSet = new ArrayList<>();
        ArrayList<Integer> set = new ArrayList<>();      
        int totalBits = n-2;
        int minCost = -1, tempCost, row;
        
        for (int i = 0; i < (int)Math.pow(2,totalBits); i++)
        {
            set = new ArrayList<>();
            set.add(0);
            for (int j = 0; j < n-2; j++)
            {
                if (((i >> j) & 0x1) != 0)
                {
                    set.add(j + 1);
                }
            }
            set.add(n-1);
            powerSet.add((ArrayList<Integer>)set.clone());
        }
        
        
        for (ArrayList<Integer> value : powerSet)
        {
            tempCost = 0;
            row = 0;
            for (int j : value)
            {
                tempCost += mat[row][j];
                row = j;
            }
            if (tempCost < minCost || minCost == -1)
            {
                minCost = tempCost;
                set = value;
            }
        }
        
        set.add(0, minCost);
        return set;
    }
    
    static
    public int[][] writeRandMatrix(final int n) throws IOException
    {
        File toWrite = new File("test" + n + ".txt");
        FileWriter wr = new FileWriter(toWrite);
        Random rand = new Random();
        int[][] randMat = new int[n][n];
        int tempRand;
        
        int toReturn[][] = new int[n][n];
        
        for (int i = 0; i < n; i++)
        {
            for (int j = 0; j < n; j++)
            {
                if (j < i)
                {
                    wr.append("NA");
                    randMat[i][j] = -1;
                }

                else if (j == i)
                {
                    wr.append("0");
                    randMat[i][j] = 0;
                }
                else
                {
                    tempRand = 1 + rand.nextInt(9);
                    wr.append(String.valueOf(tempRand));
                    randMat[i][j] = tempRand;
                }
                
                if (j < n - 1)
                    wr.append("\t");
            }
            wr.append("\n");
        }
        wr.close();
        return randMat;
    }
    
    static
    public ArrayList<Integer> divideAndConquer()
    {
        return divideAndConquer(0, 0, null);
    }
    
    static
    public ArrayList<Integer> divideAndConquer(final int totalCost,
                                                                        final int currentRow,
                                                                        ArrayList<Integer> seq)
    {
        if (seq == null)
        {
            seq = new ArrayList<>();
            seq.add(0);
        }
        
        seq.add(currentRow);
        ArrayList<Integer> toReturn = seq;
        ArrayList<Integer> tempArr;
        
        int minCost = -1;
        int tempCost;

        for (int j = currentRow+1; j < n; j++)
        {
            seq.set(0, totalCost + mat[currentRow][j]);
            tempArr = divideAndConquer(seq.get(0), j, (ArrayList<Integer>)seq.clone());
            tempCost = tempArr.get(0);
            if (tempCost < minCost || minCost == -1)
            {
                toReturn = tempArr;
                minCost = tempCost;
            }
        }
        
        return toReturn;
    }
    
    
    
    static
    public ArrayList<Integer> dynamicProgramming()
    {
        ArrayList<Integer> cheapestRow = new ArrayList<>();
        
        final ArrayList<ArrayList<Integer>> minRows = new ArrayList<>();
        cheapestRow.add(0);
        cheapestRow.add(0);
        
        minRows.add((ArrayList<Integer>)cheapestRow.clone());
        int tempMin, min;
        for (int j = 1; j < n; j++)
        {
            cheapestRow = new ArrayList<>();
            min = -1;
            for (int i = 0; i < j; i++)
            {
                tempMin = mat[i][j] + minRows.get(i).get(0);
                if (tempMin < min || min == -1)
                {
                    cheapestRow = (ArrayList<Integer>)minRows.get(i).clone();
                    cheapestRow.add(j);
                    cheapestRow.set(0, tempMin);
                    
                    min = tempMin;
                }
            }
            minRows.add((ArrayList<Integer>) cheapestRow.clone());
        }
        return minRows.get(n-1);
    }
    
}
