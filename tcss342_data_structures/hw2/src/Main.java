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
	testGenome();
	
    }
    
    public static void testGenome()
    {
	Genome test = new Genome(0.05);
	test.gene = "JESSE";
	test.addChar();
	System.out.println(test.gene);
    }
    
    public void testPopulation()
    {
	
    }
}
