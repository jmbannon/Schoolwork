/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/**
 *
 * @author jbannon
 */
public class Main {
    
    public static void main(String[] args) {
	//testGenome();
	Population temp = new Population(100, 0.05);
	int i = 0;
	
	while(!temp.isTarget()) {
	    System.out.println(i + " " + temp.mostFit.gene + " " + temp.mostFit.fitness());
	    temp.day();
	    ++i;
	}
	    
	
    }
    
    public static void testGenome()
    {
	Genome test = new Genome(0.05);
	test.gene = "JESSE";
	//test.addChar();
	test.delChar();
	System.out.println(test.isMutation());
	
	Genome test2 = new Genome(0.05);
	test2.gene = "JESSE MICHAEL BANNON";
	
	System.out.println(test2.fitness());
    }
    
    public void testPopulation()
    {
	
    }
}
