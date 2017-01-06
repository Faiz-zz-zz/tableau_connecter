if [[ "$OSTYPE" == "linux-gnu" ]]; then 
    echo "Oh I see that you are on Linux! I'll run the correct executables"
    ./bin/formula_creator
    ./prover
    ./bin/checker
elif [[ "$OSTYPE" == "darwin"* ]]; then
    echo "Oh I see that you are on MacOS! I'll run the correct executables"
    ./bin/formula_creator_mac
    ./prover
    ./bin/checker_mac
fi
