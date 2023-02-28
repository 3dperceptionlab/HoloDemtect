import qrcode, os, json

def ask_load_dict():
    while True:
        ans = input("Load dictionary or create a new one? (l/c): ").lower()
        if ans=="l" or ans=="c":
            return ans=="l"
        else:
            print("Input error. Select a valid option L/C.")

def menu():
    while True:
        print("Select one of the following options:")
        print("\t 1. See contents.")
        print("\t 2. Remove QR by id.")
        print("\t 3. Add QR.")
        print("\t 4. Edit QR dictionary.")
        print("\t 5. Generate QR image.")
        print("\t 6. Save JSON and exit.")
        option = input("Selected option: ")
        if not option.isnumeric():
            print("Input error. Option must be a number.")
        elif int(option) < 1 or int(option) > 6:
            print("Input error. Invalid option.")
        else:
            return int(option)

def qr_menu():
    while True:
        print("Select one of the following options:")
        print("\t 1. See contents.")
        print("\t 2. Add item.")
        print("\t 3. Return.")
        print("***NOTE: To remove or modify items, edit the JSON manually :)")
        option = input("Selected option: ")
        if not option.isnumeric():
            print("Input error. Option must be a number.")
        elif int(option) < 1 or int(option) > 3:
            print("Input error. Invalid option.")
        else:
            return int(option)

def print_contents(data):
    print(data)

def create_item():
    item = {}
    item['mesh'] = input("Mesh path in UE4: ")
    item['x'] = int(input("X position relative to the QR center: "))
    item['y'] = int(input("Y position relative to the QR center: "))
    item['z'] = int(input("Z position relative to the QR center: "))
    item['rx'] = int(input("X rotation relative to the QR: "))
    item['ry'] = int(input("Y rotation relative to the QR: "))
    item['rz'] = int(input("Z rotation relative to the QR: "))
    item['scale'] = int(input("Scale of the hologram: "))
    return item

def main():
    # Load/Create dict
    qr_unreal_dict = {}
    while True:
        if ask_load_dict():
            try:
                with open(input("Enter JSON filename: "), 'r') as f:
                    qr_unreal_dict = json.load(f)
                    qr_unreal_dict.pop("keys")
                break
            except FileNotFoundError:
                print("File does not exist. Creating empty dictionary.")
        else:
            break

    # Process dict
    while True:
        menu_option = menu()
        if menu_option == 1: # See contents
            print_contents(qr_unreal_dict)
        elif menu_option == 2: # Remove item
            remove_id = input("Enter id to remove: ")
            if not remove_id in qr_unreal_dict:
                print("Error. The id does not exist.")
            else:
                qr_unreal_dict.pop(remove_id)
        elif menu_option == 3: # Add item
            new_id = input("Enter the new id: ")
            if new_id in qr_unreal_dict:
                print("Error. The id already exists.")
            else:
                qr_unreal_dict[new_id] = []
        elif menu_option == 4: # Edit item
            qr_id = input("Enter id: ")
            if not qr_id in qr_unreal_dict:
                print("Error. The id does not exist.")
                continue
            while True:
                qr_menu_option = qr_menu()
                if qr_menu_option == 1:
                    print_contents(qr_unreal_dict[qr_id])
                elif qr_menu_option == 2:
                    try:
                        qr_unreal_dict[qr_id].append(create_item())
                    except ValueError:
                        print("Input error. Expected integer value.")
                elif qr_menu_option == 3:
                    break
        elif menu_option == 5: # Generate QR for item
            qr_id = input("Enter id: ")
            if not qr_id in qr_unreal_dict:
                print("Error. The id does not exist.")
            else:
                qrcode.make(qr_id).save(qr_id + ".png")
                print(f"QR saved as {qr_id}.png")
        elif menu_option == 6: # Exit and save
            break
        
    qr_unreal_dict['keys'] = list(qr_unreal_dict.keys())
    with open(os.path.join("../Content/JSONDicts/", input("Enter filename for the new JSON (saved in /Game/Content/JSONDicts): ")), 'w', encoding='utf-8') as f:
        json.dump(qr_unreal_dict, f, ensure_ascii=False, indent=4)

    print("File saved.")

if __name__ == "__main__":
    main()