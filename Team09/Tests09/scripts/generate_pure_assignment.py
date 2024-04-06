def generate_pure_assignments(num_lines):
    procedure_name = "PureAssignments"
    assignment_template = "var{} = var{};"
    lines = []

    for i in range(1, num_lines + 1):
        line = assignment_template.format(i, i - 1)
        lines.append(line)

    procedure = f"procedure {procedure_name} {{\n"
    procedure += "\n".join(lines)
    procedure += "\n}"

    return procedure

if __name__ == "__main__":
    num_lines = 300
    procedure_code = generate_pure_assignments(num_lines)
    with open("milestone3_source_5.txt", "w") as file:
        file.write(procedure_code)
