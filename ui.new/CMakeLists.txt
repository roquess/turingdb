
add_custom_command(OUTPUT timestamp
    COMMAND cd frontend && npm run build
    DEPENDS frontend/package.json
            frontend/package-lock.json)

add_custom_target(turing_ui_site DEPENDS timestamp)
