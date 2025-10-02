let app () =
  let open Lwt.Syntax in
  Printf.printf "Hello!\n%!";
  let* () = Mirage_sleep.ns 1_000_000_000L in
  Printf.printf "After 1s\n%!";
  let+ () = Mirage_sleep.ns 1_000_000_000L in
  Printf.printf "After 2s\n%!"

let () = Solo5_os.Main.run (app ())
