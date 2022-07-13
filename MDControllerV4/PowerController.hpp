//A25B42C34D34

int FLPower(String inputPower){
    int postionA = inputPower.indexOf("FL") + 1;
    int postionB = inputPower.indexOf("FR");
    if (postionA == -1){
        return 0;
    }
    if(postionB == -1){
        return 0;
    }
    if(postionB < postionA){
        return 0;
    }

    String outputObject = ""; 
    while (postionA != postionB)
    {
        outputObject += inputPower.charAt(postionA + 1);
        postionA++;
    }

    int RTobject = outputObject.toInt();
    if(RTobject > 254){
        RTobject = 254;
    }else if(RTobject < -254){
        RTobject = -254;
    }

    return RTobject; 
}

int FRPower(String inputPower){
    int postionB = inputPower.indexOf("FR") + 1;
    int postionC = inputPower.indexOf("RL");
    if (postionB == -1){
        return 0;
    }
    if(postionC == -1){
        return 0;
    }
    if(postionC < postionB){
        return 0;
    }

    String outputObject = ""; 
    while (postionB != postionC)
    {
        outputObject += inputPower.charAt(postionB + 1);
        postionB++;
    }

    int RTobject = outputObject.toInt();
    if(RTobject > 254){
        RTobject = 254;
    }else if(RTobject < -254){
        RTobject = -254;
    }

    return RTobject; 
}

int RLPower(String inputPower){
    int postionC = inputPower.indexOf("RL") + 1;
    int postionD = inputPower.indexOf("RR");
    if (postionC == -1){
        return 0;
    }
    if(postionD == -1){
        return 0;
    }
    if(postionD < postionC){
        return 0;
    }

    String outputObject = ""; 
    while (postionC != postionD)
    {
        outputObject += inputPower.charAt(postionC + 1);
        postionC++;
    }

    int RTobject = outputObject.toInt();
    if(RTobject > 254){
        RTobject = 254;
    }else if(RTobject < -254){
        RTobject = -254;
    }

    return RTobject; 
}

int RRPower(String inputPower){
    int postionD = inputPower.indexOf("RR") + 1;
    int postionE = inputPower.indexOf("END");
    if (postionD == -1){
        return 0;
    }
    if(postionE == -1){
        return 0;
    }
    if(postionE < postionD){
        return 0;
    }

    String outputObject = ""; 
    while (postionD != postionE)
    {
        outputObject += inputPower.charAt(postionD + 1);
        postionD++;
    }

    int RTobject = outputObject.toInt();
    if(RTobject > 254){
        RTobject = 254;
    }else if(RTobject < -254){
        RTobject = -254;
    }

    return RTobject; 
}
