/*
 * Author:      Jesse Bannon
 * Class:       TCSS 342B; Data Structures
 * Desc:        Mutates a string start with 'A' to its target string
 *              using artificial evolution.
 */

import java.util.ArrayList;
import java.util.Random;

public class Population 
{
    public Genome mostFit;
    private ArrayList<Genome> population;
    private final Random rand;
    private final String target;
    
    public Population(int numGenomes, 
	              double mutationRate) 
    {
	this.population = new ArrayList<>();
	this.rand = new Random();
	
	for (int i = 0; i < numGenomes; i++)
	    this.population.add(new Genome(mutationRate));
	
	this.mostFit = this.population.get(0);
        this.target = this.population.get(0).target;
    }
    
    void day() 
    {
	final int popSize = population.size();
	population = quickSort(population);
	mostFit = population.get(0);
	
	for (int i = popSize/2; i < popSize; i++) 
	{
	    final Genome temp = new Genome(this.population.get(rand.nextInt(popSize/2)));
	    if (rand.nextBoolean()) {
		temp.mutate();
	    } else {
		temp.crossover(this.population.get(rand.nextInt(popSize/2)));
		temp.mutate();
	    }
	    
	    population.set(i, temp);
	}
    }
    
    ArrayList<Genome> quickSort(ArrayList<Genome> theList) 
    {
	final int len = theList.size();

	if (len == 0)
	    return theList;
	
	ArrayList<Genome> left = new ArrayList<>();
	ArrayList<Genome> right = new ArrayList<>();
	ArrayList<Genome> pivots = new ArrayList<>();
	ArrayList<Genome> toReturn = new ArrayList<>();
	
	final int pivot = theList.get(rand.nextInt(len)).fitness();
	
	for (int i = 0; i < len; i++)
	{
	    if (theList.get(i).fitness() < pivot)
		left.add(theList.get(i));
	    else if (theList.get(i).fitness() > pivot)
		right.add(theList.get(i));
	    else
		pivots.add(theList.get(i));
	}
	
	left = quickSort(left);
	right = quickSort(right);
	
	for (Genome left1 : left)
	    toReturn.add(left1);
	
	for (Genome pivot1 : pivots)
	    toReturn.add(pivot1);
	
	for (Genome right1 : right)
	    toReturn.add(right1);
	
	return toReturn;
    }
    
    public boolean isTarget() 
    {
	return mostFit.fitness() == 0;
    }
    
}
