/*
 * Author:      Jesse Bannon
 * Class:       TCSS 342B; Data Structures
 * Desc:        Mutates a string start with 'A' to its target string
 *              using artificial evolution.
 */

import java.util.Random;

public class Genome
{
    public final String target = "JESSE MICHAEL BANNON";
    protected String gene;
    protected double mutationRate;
    private final Random rand;

    public Genome(double mutationRate)
    {
        this.gene = "A";
        this.mutationRate = mutationRate;
        this.rand = new Random();
    }

    public Genome(Genome gene)
    {
        this.gene = gene.gene;
        this.mutationRate = gene.mutationRate;
        this.rand = new Random();
    }

    public void mutate()
    {
        if (isMutation())
            addChar();

        if (isMutation())
            delChar();

        char temp[] = this.gene.toCharArray();
        for (int i = 0; i < temp.length; i++)
        {
            if (isMutation())
                temp[i] = randChar();
        }
        this.gene = new String(temp);
    }

    public void crossover(Genome other)
    {
        final int lessLen, greatLen;
        final boolean otherIsGreater;
        final StringBuilder temp = new StringBuilder();

        if (this.gene.length() > other.gene.length())
        {
            greatLen = this.gene.length();
            lessLen = other.gene.length();
            otherIsGreater = false;
        }
        else
        {
            greatLen = other.gene.length();
            lessLen = this.gene.length();
            otherIsGreater = true;
        }

        int i = 0;
        while (i < lessLen)
        {
            if (this.rand.nextBoolean())
                temp.append(this.gene.charAt(i++));
            else
                temp.append(other.gene.charAt(i++));
        }

        while (i < greatLen)
        {
            if (this.rand.nextBoolean())
            {
                if (otherIsGreater)
                    temp.append(other.gene.charAt(i++));
                else
                    temp.append(this.gene.charAt(i++));
            }
            else
                break;
        }
        this.gene = temp.toString();
    }

    public int fitness()
    {
        final int geneLen = this.gene.length();
        final int targetLen = this.target.length();

        final boolean targetIsGreater = (targetLen > geneLen);
        final int lessLen = targetIsGreater ? geneLen : targetLen;
        final int greatLen = targetIsGreater ? targetLen : geneLen;

        int fitness = Math.abs(targetLen - geneLen);

        int i = 0;
        while (i < lessLen)
        {
            if (this.gene.charAt(i) != this.target.charAt(i))
            ++fitness;
            ++i;
        }
        return fitness += (greatLen - i);
    }

    private int min(final int x, final int y, final int z)
    {
        if (x >= y && x >= z)
            return x;
        else if (y >= x && y >= z)
            return y;
        else if (z >= x && z >= y)
            return z;
        else
            System.out.println("ERROR, RETURNING 0");
        return 0;
    }

    // LEVENSHTEIN EDIT-DISTANCE
    /*
    public int fitness()
    {
        final int n = this.gene.length();
        final int m = this.target.length();
        int D[][] = new int[n+1][m+1];
        D[0][0] = 0;

        for (int i = 0; i <= m; i++)
            D[0][i] = i;
        for (int i = 0; i <= n; i++)
            D[i][0] = i;

        for (int i = 1; i <= n; i++)
        {
            for (int j = 1; j <= m; j++)
            {
                if (this.gene.charAt(i-1) == this.target.charAt(j-1))
                    D[i][j] = D[i-1][j-1];
                else
                    D[i][j] = this.min(D[i-1][j]+1, D[i][j-1]+1, D[i-1][j-1]+1);
            }
        }
        return D[n][m] + (int)((Math.abs(n-m) +1)/2);
    }
    */

    public boolean isMutation()
    {
        final double max = mutationRate * 100000;
        final int determinant = this.rand.nextInt(100000);

        return max > determinant;
    }

    public void delChar()
    {
        final int len = this.gene.length();
        if (len <= 2)
            return;

        final int delIndex = this.rand.nextInt(len);

        this.gene = this.gene.substring(0, delIndex)
            + this.gene.substring(delIndex + 1, len);
    }

    public void addChar()
    {
        final int len = this.gene.length();
        final int split = this.rand.nextInt(len+1);

        this.gene = this.gene.substring(0, split)
            + this.randChar()
            + this.gene.substring(split, len);
    }

    private char randChar()
    {
        final int randomNum = this.rand.nextInt(29);
        switch(randomNum)
        {
            case 26: return ' ';
            case 27: return '-';
            case 28: return '\'';
            default: return (char)('A' + randomNum);
        }
    }
}
