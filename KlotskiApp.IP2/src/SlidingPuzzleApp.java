/**
Author: Peng
*/
import javax.swing.JFrame;
import javax.swing.JPanel;
import javax.swing.border.EmptyBorder;
import javax.swing.GroupLayout;
import javax.swing.GroupLayout.Alignment;
import java.awt.Dimension;
import java.awt.Color;
import javax.swing.JButton;
import javax.swing.LayoutStyle.ComponentPlacement;
import javax.swing.JLabel;
import javax.swing.SwingConstants;
import javax.swing.JTextArea;
import java.awt.Font;

public class SlidingPuzzleApp extends JFrame {

	private JPanel contentPane;
	Model model;
	PuzzleView panel;
	JLabel label= new JLabel("0");
	PuzzleMouseController mouseController;
	PuzzleKeyController keyController;
	PuzzleButtonController buttonController;
	public PuzzleView getPuzzleView() { return panel; }
	public JLabel getLabel() { return label; }

	/**
	 * Create the frame.
	 */
	public SlidingPuzzleApp(Model model) {
		setResizable(false);   // make sure can't resize
		this.model = model; //pass in model
		
		setTitle("SlidingPuzzleApp");
		setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);//TODO: exit prompt
		setBounds(100, 100, 600, 600);
		contentPane = new JPanel();
		contentPane.setBorder(new EmptyBorder(5, 5, 5, 5));
		setContentPane(contentPane);
		
		//get puzzle from model, set to puzzle view
		panel = new PuzzleView(model.getPuzzle());
		panel.setBackground(Color.white);
		panel.setSize(new Dimension(401, 500));
		
		//>>>>>>add listen<<<<<<<<<<
//		puzzleView = new PuzzleView(model.getPuzzle());    // give it the puzzle to display
		mouseController = new PuzzleMouseController(this, model);
		keyController = new PuzzleKeyController(this, model);
		buttonController = new PuzzleButtonController (this, model);

		panel.addMouseListener(mouseController);
		panel.addKeyListener(keyController); 
		
		
		JButton btnReset = new JButton("Reset");
		btnReset.setFont(new Font("Tahoma", Font.PLAIN, 15));
		btnReset.addActionListener(buttonController);

		/*use button to move tiles*/
//		button.addActionListener(
//				new ActionListener(){
//					@Override
//					public void actionPerformed(ActionEvent e) {
//						if(panel.puzzle.moveSelected('w')) {
//							panel.repaint();
//						}
//					}
//				});
//		button_1.addActionListener(
//				new ActionListener(){
//					@Override
//					public void actionPerformed(ActionEvent e) {
//						if(panel.puzzle.moveSelected('a')) {
//							panel.repaint();
//						}
//					}
//				});
//		button_2.addActionListener(
//				new ActionListener(){
//					@Override
//					public void actionPerformed(ActionEvent e) {
//						if(panel.puzzle.moveSelected('d')) {
//							panel.repaint();
//						}
//					}
//				});
//		btnV.addActionListener(
//				new ActionListener(){
//					@Override
//					public void actionPerformed(ActionEvent e) {
//						if(panel.puzzle.moveSelected('s')) {
//							panel.repaint();
//						}
//					}
//				});
		
		JLabel lblMoves = new JLabel("Moves:");
		lblMoves.setFont(new Font("Tahoma", Font.PLAIN, 17));
		
		JLabel lblExitHere = new JLabel("EXIT HERE");
		
		JTextArea txtrReferToReadme = new JTextArea();
		txtrReferToReadme.setText("Refer to README \r\nfor more info");
		
//		System.out.print(s);

		GroupLayout gl_contentPane = new GroupLayout(contentPane);
		gl_contentPane.setHorizontalGroup(
			gl_contentPane.createParallelGroup(Alignment.LEADING)
				.addGroup(gl_contentPane.createSequentialGroup()
					.addGroup(gl_contentPane.createParallelGroup(Alignment.LEADING)
						.addGroup(gl_contentPane.createSequentialGroup()
							.addContainerGap()
							.addComponent(panel, GroupLayout.PREFERRED_SIZE, 401, GroupLayout.PREFERRED_SIZE)
							.addGroup(gl_contentPane.createParallelGroup(Alignment.LEADING)
								.addGroup(gl_contentPane.createSequentialGroup()
									.addGap(40)
									.addComponent(btnReset, GroupLayout.PREFERRED_SIZE, 93, GroupLayout.PREFERRED_SIZE))
								.addGroup(gl_contentPane.createSequentialGroup()
									.addGap(38)
									.addComponent(lblMoves)
									.addGap(18)
									.addComponent(label))
								.addGroup(gl_contentPane.createSequentialGroup()
									.addGap(57)
									.addComponent(txtrReferToReadme, GroupLayout.PREFERRED_SIZE, 120, GroupLayout.PREFERRED_SIZE)))
							.addGap(291))
						.addGroup(gl_contentPane.createSequentialGroup()
							.addGap(186)
							.addComponent(lblExitHere)))
					.addContainerGap())
		);
		gl_contentPane.setVerticalGroup(
			gl_contentPane.createParallelGroup(Alignment.LEADING)
				.addGroup(gl_contentPane.createSequentialGroup()
					.addContainerGap()
					.addGroup(gl_contentPane.createParallelGroup(Alignment.LEADING)
						.addComponent(panel, GroupLayout.DEFAULT_SIZE, 527, Short.MAX_VALUE)
						.addGroup(gl_contentPane.createSequentialGroup()
							.addGroup(gl_contentPane.createParallelGroup(Alignment.BASELINE)
								.addComponent(lblMoves)
								.addComponent(label))
							.addGap(156)
							.addComponent(txtrReferToReadme, GroupLayout.PREFERRED_SIZE, 108, GroupLayout.PREFERRED_SIZE)
							.addPreferredGap(ComponentPlacement.RELATED, 175, Short.MAX_VALUE)
							.addComponent(btnReset, GroupLayout.PREFERRED_SIZE, 46, GroupLayout.PREFERRED_SIZE)))
					.addPreferredGap(ComponentPlacement.RELATED)
					.addComponent(lblExitHere)
					.addContainerGap())
		);
		contentPane.setLayout(gl_contentPane);
	}
}
