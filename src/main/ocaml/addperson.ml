open Core.Std

module A = Addressbook_piqi
module A_ext = Addressbook_piqi_ext

let add_person filename =
  print_endline "Adding a silly person...";
  let book =
    let s = In_channel.read_all filename in
    A_ext.parse_address_book s `json in
  let count = List.length book.A.Address_book.person in
  let newp = A.Person.({ (A.default_person ()) with
                         id = Int32.of_int_exn (count + 1);
                         name = "Silly Person"
                       }) in
  let newb = A.Address_book.({ person = newp :: book.person }) in
  let opts = Piqirun_ext.make_options ~pretty_print:true () in
  let json = A_ext.gen_address_book ~opts newb `json in
  print_endline json;
  Out_channel.write_all filename ~data:json

let spec =
  let open Command.Spec in
  empty
  +> anon ("filename" %: string)

let command =
  Command.basic
    ~summary:"Add a silly person to testdata.json"
    spec
    (fun filename () -> add_person filename)

let () =
  Command.run ~version:"1.0" ~build_info:"piqi experiment" command
