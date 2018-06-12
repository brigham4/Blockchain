import static java.lang.Math.pow;
//This program is to calculate the relative loss of a mining pool in which there
//is a selfish miner in the environment.

public class calculate_loss{
    //comp power of selfish miner
    public float alpha;
    //comp power of honest miner
    public float beta;
    //gamma
    public float gamma;

    public calculate_loss(float a, float b, float g){
        this.alpha = a;
        this.beta = b;
        this.gamma = g;
    }   
    public float calculate(){
        float p0 = (float) ( (float) (alpha - 2*(Math.pow(alpha, 2))))/( (float) (alpha*(2*Math.pow(alpha, 3) - (4*(Math.pow(alpha, 2)) + 1))));
        System.out.println("p-naught is: " + p0);
        float p1 = (float) ( (float) (alpha - 2*(Math.pow(alpha, 2))))/( (float) (2*Math.pow(alpha, 3) - (4*(Math.pow(alpha, 2)) + 1)));
        System.out.println("p-1 is: " + p1);
        float p0p = (float) ( (float) (1 - alpha)*(alpha - 2*(Math.pow(alpha, 2))))/( (float) (1 - 4*(Math.pow(alpha, 2) + 2*(Math.pow(alpha, 3)))));
        System.out.println("p-naught-prime is: " + p0p);
        float prob_loss = (beta * (1 - p0 - p1)) + ((beta/(1-alpha))*p0p)*(alpha + (1-gamma) * (1 - alpha - beta));
        float proportion_lost = (beta-prob_loss)/beta;
        System.out.println("The probability that the block is lost is: " + prob_loss);
        System.out.println("The proportion that the honest miner loses is: " + proportion_lost);
        return prob_loss;
    }
    public static void main(String[] args){
        System.out.println("alpha read in: " + args[0]);
        System.out.println("beta read in: " + args[1]);
        System.out.println("gamma read in: " + args[2]);
        float alpha = Float.parseFloat(args[0]); 
        float beta = Float.parseFloat(args[1]);
        float gamma = Float.parseFloat(args[2]);
            calculate_loss calc = new calculate_loss(alpha, beta, gamma);
            calc.calculate();
    }
}