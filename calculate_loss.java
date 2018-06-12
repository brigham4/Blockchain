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
        float p1 = (float) ( (float) (alpha - 2*(Math.pow(alpha, 2))))/( (float) (2*Math.pow(alpha, 3) - (4*(Math.pow(alpha, 2)) + 1)));
        float p0p = (float) ( (float) (1 - alpha)*(alpha - 2*(Math.pow(alpha, 2))))/( (float) (1 - 4*(Math.pow(alpha, 2) + 2*(Math.pow(alpha, 3)))));
        float prob_loss = (beta * (1 - p0 - p1)) + ((beta/(1-alpha))*p0p)*(alpha + (1-gamma) * (1 - alpha - beta));
        float proportion_lost = (beta-prob_loss)/beta;
        System.out.println(prob_loss);
        System.out.println(proportion_lost);
        return prob_loss;
    }
    public static void main(String[] args){
        float alpha = Float.parseFloat(args[0]); 
        float beta = Float.parseFloat(args[1]);
        float gamma = Float.parseFloat(args[2]);
            calculate_loss calc = new calculate_loss(alpha, beta, gamma);
            calc.calculate();
    }
}