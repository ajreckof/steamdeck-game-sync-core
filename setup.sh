#!/usr/bin/env bash

read -rp "Project name: " PROJECT_NAME


FILES=(
    "demo/addons/project_name_placeholder/bin/project_name_placeholder.gdextension"
    "SConstruct"
    ".github/workflows/builds.yml"
)

for FILE in "${FILES[@]}"; do
    sed "s|project_name_placeholder|${PROJECT_NAME}|g" "${FILE}" | cat > temp && mv temp "${FILE}"
done
mv demo/addons/project_name_placeholder/bin/project_name_placeholder.gdextension "demo/addons/project_name_placeholder/bin/${PROJECT_NAME}.gdextension"
mv demo/addons/project_name_placeholder "demo/addons/${PROJECT_NAME}"

