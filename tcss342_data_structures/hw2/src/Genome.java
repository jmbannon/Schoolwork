
import java.util.Arrays;
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
    public final String target = "JESSE MICHAEL BANNON";
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
	this.rand = new Random();
    }
    
    public void mutate() {
	final int len = this.gene.length();
	
	if (isMutation())
	    addChar();
	
	if (isMutation())
	    delChar();
	
	char temp[] = this.gene.toCharArray();
	for (char c : temp)
	{
	    if (isMutation())
	    {
		c = randChar();
	    }
	}
	
	this.gene = new String(temp);
	//System.out.println("mutate: " + this.gene);
    }
    
    public void crossover(Genome other) {
	final int lessLen, greatLen;
	final boolean otherIsGreater;
	
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
	
	StringBuilder temp = new StringBuilder();

	int i = 0;
	while (i < lessLen) 
	{
	    //System.out.println(lessLen + "gene size = " + gene.length() + " other len = " + other.gene.length());
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
    
    int fitness() {
	final int geneLen = this.gene.length();
	final int targetLen = this.target.length();
	
	final boolean targetIsGreater = (targetLen > geneLen);	
	final int lessLen = targetIsGreater ? geneLen : targetLen;
	
	int fitness = Math.abs(targetLen - geneLen);
	
	int i = 0;
	while (i < lessLen)
	{
	    if (this.gene.charAt(i) != this.target.charAt(i))
		++fitness;
	    ++i;
	}
	return fitness;
    }
    
    public boolean isMutation() {
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
