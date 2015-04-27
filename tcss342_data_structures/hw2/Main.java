/*
 * Author:      Jesse Bannon
 * Class:       TCSS 342B; Data Structures
 * Desc:        Mutates a string start with 'A' to its target string
 *              using artificial evolution.
 */

public class Main 
{
    public static void main(String[] args) 
    {    
        final Population pop = new Population(100, 0.05);
        int i = 0;
        
        final long startTime = System.currentTimeMillis();
        while(!pop.isTarget()) 
        {
            System.out.println("\"" 
                    + pop.mostFit.gene 
                    + "\" " 
                    + pop.mostFit.fitness());
            pop.day();
            ++i;
        }
        final long endTime = System.currentTimeMillis();
        
        System.out.println("-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-");
        System.out.println("Target:\t\t" + pop.mostFit.gene);
        System.out.println("Generations:\t" + i);
        System.out.println("Time:\t\t" + (endTime-startTime) + "ms");	
    }
    
    public static void testGenome()
    {
        Genome test = new Genome(0.05);
        test.gene = "JESSE";
        test.addChar();
        test.delChar();
        System.out.println(test.isMutation());
        System.out.println(test.gene);
        
        Genome test2 = new Genome(0.05);
        test2.gene = "JESSE MICHAEL BANNON";
	
        System.out.println(test2.fitness());
    }
    
    public void testPopulation()
    {
	    Population test = new Population(100, 0.05);
        
        for (int i = 0; i < 50; i++)
        {
            test.day();
            System.out.println(test.mostFit.gene);
        }
    }
}
