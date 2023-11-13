#!/bin/bash

runs='1'

print_usage() {
  printf "Usage: -n <number of runs>"
}

while getopts 'n:v' flag; do
    case "${flag}" in
        n) runs="${OPTARG}" ;;
        *) print_usage
           exit 1 ;;
    esac
done

subfolder="$(date "+%Y.%m.%d-%H.%M.%S")_"$runs"runs"

# copy codebase to camp
ssh -v login002.nemo.thecrick.org mkdir -p acemodel/$subfolder

# rsync -rv --include='*.'{cpp,h} --include='src' --include='src/**' --include='extern' --include='extern/gcem' --include='extern/gcem/**' --include='CMakeLists.txt' --include='build.sh' --include='camp' --include='camp/slurm.sh' --exclude="*" --delete-excluded ./ login002.nemo.thecrick.org:~/acemodel/$subfolder/

# build project on camp and start job array. Wait for jobs to finish.
ssh -v login002.nemo.thecrick.org \
<< EOF
cd acemodel/$subfolder
ml purge
ml foss
ml CMake/3.24.3-GCCcore-11.3.0
ml Boost
./build.sh release
sbatch --array 1-$runs --wait camp/slurm.sh
EOF

# copy output to local folder
mkdir -p camp/outputs/$subfolder
rsync -rv login002.nemo.thecrick.org:~/acemodel/$subfolder/output ./camp/outputs/$subfolder/
