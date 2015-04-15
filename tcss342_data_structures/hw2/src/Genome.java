
import java.util.Random;

/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/**
 *
 * @author jbannon
 */
public class Genome {
    
    protected String gene;
    protected double mutationRate;
    private Random rand;
    
    public Genome(double mutationRate) {
	this.gene = "A";
	this.mutationRate = mutationRate;
	this.rand = new Random();
    }
    
    public Genome(Genome gene) {
	this.gene = gene.gene;
	this.mutationRate = gene.mutationRate;
    }
    
    void mutate() {
	
    }
    
    void crossover(Genome other) {
	
    }
    
    int fitness() {
	return 0;
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
